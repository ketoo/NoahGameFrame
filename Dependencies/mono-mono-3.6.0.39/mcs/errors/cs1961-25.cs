// CS1961: The covariant type parameter `T' must be invariantly valid on `B<T>()'
// Line: 8

interface A<T>
{
}

delegate A<A<T>> B<out T> ();
