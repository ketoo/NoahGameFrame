#include "ctb-0.16/ctb.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <sstream>
#include <string>
#include "httppil.h"
#include "httpapi.h"

using namespace std;
vector<ctb::SerialPort*> serials;
ctb::SerialPort* serialPort = 0;

extern "C" int uhSerial(UrlHandlerParam* param)
{

	char* port = mwGetVarValue(param->pxVars, "port", "");
	vector<ctb::SerialPort*>::iterator it;
	int size = serials.size();
	//cerr << "[SERIAL] " << size << " opened ports" << endl;
	for (it = serials.begin() ; it < serials.end(); it++) {
		char *p = (*it)->m_devname;
		if (!strncmp(p, "\\\\.\\", 4)) {
			p += 4;
		}
		if (!strcmp(p, port)) {
			serialPort = *it;
			break;
		}
	}

    if (!*param->pucRequest) {
        vector<string> ports;
        stringstream s;
		s << "<?xml version=\"1.0\"?>"
			<< "<SerialPorts>";
        if( ctb::GetAvailablePorts( ports )) {
			for (it = serials.begin() ; it < serials.end(); it++) {
				char *p = (*it)->m_devname;
				if (!strncmp(p, "\\\\.\\", 4)) {
					p += 4;
				}
				s << "<Port open=\"1\">" << p << "</Port>";
			}
            for( int i = 0; i < ports.size(); i++) {
				s << "<Port>" << ports[i] << "</Port>";
            }
        } else {
            s << "<Error/>";
        }
		s << "</SerialPorts>";
		param->dataBytes = snprintf(param->pucBuffer, param->dataBytes, "%s", s.str().c_str());
		param->fileType=HTTPFILETYPE_XML;
	} else if (!strcmp(param->pucRequest, "/read")) {
		int timeout = mwGetVarValueInt(param->pxVars, "timeout", 0);
		int bytes = mwGetVarValueInt(param->pxVars, "bytes", param->dataBytes);
		char* eos = mwGetVarValue(param->pxVars, "eos", 0);
		serialPort->SetTimeout(timeout);
		if (eos) {
			if (serialPort->ReadUntilEOS(param->pucBuffer, (size_t*)&param->dataBytes, eos, timeout) == -1) {
                param->dataBytes = 0;
			}
		} else {
			param->dataBytes = serialPort->Read(param->pucBuffer, bytes);
		}
		cerr << "[" << serialPort->m_devname << "] " << param->dataBytes << " bytes read" << endl;
		if (param->dataBytes == 0) {
			param->dataBytes = sprintf(param->pucBuffer, "Timeout");
			param->hs->response.statusCode = 503;
		} else if (param->dataBytes < 0) {
			param->dataBytes = sprintf(param->pucBuffer, "Error");
			param->hs->response.statusCode = 502;
		} else {
			param->pucBuffer[param->dataBytes] = 0;
		}
		param->fileType=HTTPFILETYPE_TEXT;
	} else if (!strcmp(param->pucRequest, "/write") &&
		param->hs->request.payloadSize > 0 &&
		param->hs->request.payloadSize == param->hs->dataLength) {
		if (serialPort) {
			int payloadSize = param->hs->request.payloadSize;
			int written = 0;
			int timeout = mwGetVarValueInt(param->pxVars, "timeout", 1000);
			int delay = mwGetVarValueInt(param->pxVars, "delay", 0);
			char* response = mwGetVarValue(param->pxVars, "response", 0);
			if (mwGetVarValueInt(param->pxVars, "echo", 0)) {
				response = param->pucPayload;
			}
			if (response) {
				int l = strlen(response);
				serialPort->SetTimeout(timeout);
				for (int i = 0; i < payloadSize; i++, written++) {
					if (serialPort->Write(param->pucPayload + i, 1) != 1) {
						param->hs->response.statusCode = 503;
						break;
					}
					do {
						char c;
						int ret = serialPort->Read(&c, 1);
						if (ret != 1) {
                            msleep(10);
						    ret = serialPort->Read(&c, 1);
						}
						if (ret != 1) {
							param->hs->response.statusCode = 504;
							break;
						} else if (c == response[i]) {
							// matched
							break;
						}
					} while (param->hs->response.statusCode != 504);
				}
			} else if (delay == 0) {
				written = serialPort->Write(param->pucPayload, payloadSize);
			} else {
				for (int i = 0; i < payloadSize; i++, written++) {
					if (serialPort->Write(param->pucPayload + i, 1) != 1) {
						break;
					}
					msleep(delay);
				}
			}
			param->dataBytes = sprintf(param->pucBuffer, "%d", written);
			cerr << "[" << serialPort->m_devname << "] " << written << " bytes written" << endl;
			if (written <= 0) {
				param->hs->response.statusCode = 504;
			}
		} else {
			param->dataBytes = sprintf(param->pucBuffer, "No port opened");
			param->hs->response.statusCode = 503;
		}
		param->fileType=HTTPFILETYPE_TEXT;
    } else if (!strcmp(param->pucRequest, "/open")) {
        int baudrate = mwGetVarValueInt(param->pxVars, "baudrate", 9600);
        char* proto = mwGetVarValue(param->pxVars, "protocol", "8N1");
		char sport[16];
		snprintf(sport, sizeof(sport), isdigit(port[3]) && atoi(port + 3) >= 10 ? "\\\\.\\%s" : "%s", port);
		if (!serialPort) {
			bool success = false;
            serialPort = new ctb::SerialPort();
			if(success = (serialPort->Open(sport, baudrate, proto, ctb::SerialPort::NoFlowControl ) >= 0)) {
				param->dataBytes = sprintf(param->pucBuffer, "OK - %s opened", port);
			} else {
				param->dataBytes = sprintf(param->pucBuffer, "Error opening %s", port);
				param->hs->response.statusCode = 503;
			}
			if (!success) {
				delete serialPort;
				serialPort = 0;
			} else {
				serials.push_back(serialPort);
				cerr << "[" << serialPort->m_devname << "] Port opened" << endl;
			}
		} else {
			param->dataBytes = sprintf(param->pucBuffer, "%s already opened", port);
		}
		param->fileType=HTTPFILETYPE_TEXT;
    } else if (!strcmp(param->pucRequest, "/setline") && serialPort) {
		int state = mwGetVarValueInt(param->pxVars, "DTR", -1);
		if (state == 1) {
			serialPort->SetLineState(ctb::LinestateDtr);
		} else if (state == 0) {
			serialPort->ClrLineState(ctb::LinestateDtr);
		}
		state = mwGetVarValueInt(param->pxVars, "RTS", -1);
		if (state == 1) {
			serialPort->SetLineState(ctb::LinestateRts);
		} else if (state == 0) {
			serialPort->ClrLineState(ctb::LinestateRts);
		}
		state = mwGetVarValueInt(param->pxVars, "CTS", -1);
		if (state == 1) {
			serialPort->SetLineState(ctb::LinestateCts);
		} else if (state == 0) {
			serialPort->ClrLineState(ctb::LinestateCts);
		}
		cerr << "[" << serialPort->m_devname << "] Line set" << endl;
		param->dataBytes = sprintf(param->pucBuffer, "OK");
    } else if (!strcmp(param->pucRequest, "/close")) {
        if (serialPort) {
            cerr << "[" << serialPort->m_devname << "] Port closed" << endl;
			for (it = serials.begin() ; it < serials.end(); it++) {
				if (*it == serialPort) {
					serials.erase(it);
					delete serialPort;
					serialPort = 0;
					break;
				}
			}
            param->dataBytes = sprintf(param->pucBuffer, "Port closed");
        } else {
            param->dataBytes = sprintf(param->pucBuffer, "No port opened");
			param->hs->response.statusCode = 503;
        }
		param->fileType=HTTPFILETYPE_TEXT;
    } else {
        return 0;
    }
    return FLAG_DATA_RAW;
}
