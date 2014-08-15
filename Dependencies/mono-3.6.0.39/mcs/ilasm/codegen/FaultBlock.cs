//
// Mono.ILASM.FaultBlock
//
// Author(s):
//  Jackson Harper (Jackson@LatitudeGeo.com)
//
// (C) 2003 Jackson Harper, All rights reserved
//


using System;

namespace Mono.ILASM {

        public class FaultBlock : ISehClause {

                private HandlerBlock handler_block;

                public FaultBlock ()
                {

                }

                public void SetHandlerBlock (HandlerBlock hb)
                {
                        handler_block = hb;
                }

                public PEAPI.HandlerBlock Resolve (CodeGen code_gen, MethodDef method)
                {
                        PEAPI.CILLabel from = handler_block.GetFromLabel (code_gen, method);
                        PEAPI.CILLabel to = handler_block.GetToLabel (code_gen, method);
                        PEAPI.Fault fault = new PEAPI.Fault (from, to);

                        return fault;
                }
        }

}


