# ATLAS GSoC Test

## Usage
Compile: make
Execute:
    ./main -option file
    
    Options can be:
    -rc : run-length compress
    -rd : run-length decompress
    -dc : dictionary compress
    -dd : dictionary decompress


## The compression algorithms
#### Run Length Encoding

Is quicker.
Strength: files where a same value is repeated together many times (aaaaaaaabbbbb).
    - Simple icon image (with plain colors)
    - ascii_art.txt (example file)

Weakness: any file that doesn't repeat the values together, even if a same
value appears many times (abaabababa). It will probably create a compressed
file that is bigger than the original (even twice as big).
    - A photograph
    - random.txt

#### Dictionary encoding

Has a higher compression ratio.
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
- The compression rate test is done comparing the original and compressed files
- The time test is done using the chrono library
- The memory test is done externally using valgrind

#### Values for RLE for the memory test:
Compression:
- ascii_art.txt: 73,808 bytes allocated (5 allocs, 5 frees)
- random.txt: 73,728 bytes allocated (2 allocs, 2 frees)
- natural_language.txt: 73,982 bytes allocated (12 allocs, 12 frees)

Decompression:
- ascii_art.txt: 73,782 bytes allocated (5 allocs, 5 frees)
- random.txt: 73,728 bytes allocated (2 allocs, 2 frees)
- natural_language.txt: 73,824 bytes allocated (6 allocs, 6 frees)

---

TO FIX:
- [ ] Dictionary decompression not working (not reading files correctly)
