package kshark

/**
 * Can open [RandomAccessSource] instances.
 */
interface RandomAccessSourceProvider {
    fun openRandomAccessSource(): RandomAccessSource
}
