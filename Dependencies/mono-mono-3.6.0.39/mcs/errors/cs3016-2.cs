// CS3016: Arrays as attribute arguments are not CLS-compliant
// Line: 6
// Compiler options: -warnaserror -warn:1

using System;
[assembly:CLSAttribute (new bool [] {true, false})]
[assembly:CLSCompliant (true)]

public class CLSAttribute: Attribute {
        public CLSAttribute () {
        }
        
        public CLSAttribute(bool[] array) {
        }
}
