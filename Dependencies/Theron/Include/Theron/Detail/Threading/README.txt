
This folder contains implementations of simple multithreading primitives.
The idea here is to be able to support different implementations under the
hood, through a simple common API. The provided implementations support boost::thread,
std::thread and Windows threads out-of-the-box. Other implementations are possible,
and could be made publically available.

