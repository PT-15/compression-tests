# ATLAS GSoC Test

Disclaimer: no AIs used in the making of this program

## Usage
Compilation: `make`
Execution:
    ./main -option file (or just ./main to get the help printed)

In debugging function graph_to_img(_with_code), to transform .gv dot file to
img use `dot -T png -O graph.gv`


## The compression algorithms
#### Run Length Encoding
It was in general faster than dictionary encoding in the tests.

Strength: files where a same value is repeated together many times (aaaaaaaabbbbb).
    - Simple icon image (with plain colors)
    - ascii_art.txt (example file)

Weakness: any file that doesn't repeat the values together, even if a same
value appears many times (abaabababa). It will create a compressed
file that is bigger than the original (even twice as big).
    - A photograph
    - random.txt

#### Dictionary encoding
For the file itself it achieves a much higher compression ratio in general, but
it has to encode the dictionary as well (in this implementation the dictionary
is dumpled directly as chars, so it occupies a lot).

Strength: file where the frequency differences of the values are noticeable.
    - Natural language ('e' is repeated much more than 'z')
    - natural_language.txt (example file)

Weakness: files where there are lots of different values with practically the
same frequency (can end up using more space than the original file)
    - File with a big range of random numbers
    - random.txt (example file)

Still, it's a powerful compression method, used as a base for many complex
compression algorithms.


## Tests
`./main -t files`
- The compression rate test is done comparing the original and compressed files
- The time test is done using the chrono library
- The memory test is done externally using valgrind

#### ascii_art.txt
RLE compression: 73,808 bytes allocated (5 allocs, 5 frees)
Dictionary compression: 76,616 bytes allocated (74 allocs, 74 frees)

RLE decompression: 73,782 bytes allocated (5 allocs, 5 frees)
Dictionary decompression: 73,787 bytes allocated (32 allocs, 32 frees)

#### natural_language.txt
RLE compression: 73,982 bytes allocated (12 allocs, 12 frees)
Dictionary compression: 84,038 bytes allocated (245 allocs, 245 frees)

RLE decompression: 73,824 bytes allocated (6 allocs, 6 frees)
Dictionary decompression: 76,692 bytes allocated (90 allocs, 90 frees)

#### random.txt
RLE compression: 73,728 bytes allocated (2 allocs, 2 frees)
Dictionary compression: 76,296 bytes allocated (65 allocs, 65 frees)

RLE decompression: 73,728 bytes allocated (2 allocs, 2 frees)
Dictionary decompression: 73,656 bytes allocated (26 allocs, 26 frees)

