using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NFSDK
{
    class NFCRingBuffer
    {
        // 总数据段长度
        public int MaxLength { get; private set; }
        // 单次数据段最大长度
        public int MaxPackLength { get; private set; }
        public byte[] Buffer { get; private set; }
        private int _readPos = 0;
        private int _writePos = 0;

        public NFCRingBuffer(int maxLength, int maxPackLength)
        {
            MaxLength = maxLength;
            MaxPackLength = maxPackLength;
            Buffer = new byte[MaxLength + MaxPackLength];
        }

        public bool PushData(byte[] buffer, int nOffset, int length)
        {
            if (WriteSize < length)
            {
                return false;
            }

            Array.Copy(buffer, nOffset, Buffer, _writePos, length);
            WritePos = _writePos + length;
            return true;
        }
        public void Reset()
        {
            _readPos = 0;
            _writePos = 0;
        }

        public int WritePos
        {
            set
            {
                if (value > MaxLength)
                {
                    _writePos = value - MaxLength;
                }
                else
                {
                    _writePos = value;
                }
            }
            get
            {
                return _writePos;
            }
        }

        public int WriteSize
        {
            private set { }
            get
            {
                int nLength = 0;
                if (_readPos > _writePos)
                {
                    nLength = _readPos - _writePos;
                }
                else
                {
                    nLength = MaxLength - _writePos + _readPos;
                }
                return nLength < MaxPackLength ? 0 : Math.Min(MaxPackLength, nLength);
            }
        }

        public int ReadPos
        {
            set
            {
                if (value > MaxLength)
                {
                    _readPos = value - MaxLength;
                }
                else
                {
                    _readPos = value;
                }
            }
            get
            {
                return _readPos;
            }
        }
        public int ReadSize
        {
            private set { }
            get
            {
                int length = 0;
                if (_readPos > _writePos)
                {
                    length = _writePos + (MaxLength - _readPos);
                }
                else
                {
                    length = _writePos - _readPos;
                }
                return Math.Min(length, MaxPackLength);
            }
        }
    }
}