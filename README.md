ENTROPY(1)		FreeBSD General Commands Manual 	    ENTROPY(1)


# NAME
```
  entropy - calculate data entropy
```


# SYNOPSIS
```
entropy  [ -sv ] [ -tstring ]
```

# DESCRIPTION
  This manual describes the operating procedure for the entropy program.
  This program will calculate the specific entropy for a given chunk of
  binary or ascii data that was previously read from stdin or supplied on
  the command line via -t.

  The most common use for this program is to gauge the efficiency of a
  compression algorithm. In theory, based on the data's entropy, the
  maximum theoretical compression rate can be calculated.
  To understand how compression works, we must understand that all data
  can be characterized by informational content, called its entropy (the
  term is borrowed from Thermo-Dynamics).	Compression is possible
  because most data is represented with more bits then its entropy
  suggests is optimal.

# OPTIONS
```
  -t string
```
	       Rather then read the data from the stdin input stream, use the
	       string string as the source of the data. This is useful when
	       you would like to calculate for a specific word or sentence.
# THEORY

  A data set's entropy is the sum of each distinct symbol's entropies.  A
  symbol is a distinct set of contiguous bits. For the sake of
  simplicity, I use 8 (number of bits making up a byte). It is important
  to note that a symbol does NOT have to consist of 8 bits.  A distinct
  symbols entropy `S' of symbol 'z' can be defined as:

```

			     Sz = -log2(Pz)
```

  Where `Pz' is the probability of symbol 'z' being found in the set.  By
  storing each distinct symbol as a node in a list, we can calculate
  exactly how many times the symbol occurs in the set. This is also known
  as solving for the symbol's frequency.

  In order to calculate the overall entropy of the data; we sum the total
  entropies contributed by each distinct symbol and divide that by a
  coefficient known as the data size. This coefficient is calculated by
  taking the number of bits per symbol and multiplying it by the total
  quantity of symbols.

# DIAGNOSTICS
  The entropy utility exits 0 on success, and non-zero if an error
  occurs.

# SEE ALSO

```
  gzip(1), bzip2(1), tar(1), minigzip(1), gzexe(1), kgzip(8)
```
  compress(1), zcmp(1), zdiff(1), zlib(3)
```

# AUTHOR

	o Christian S.J. Peron

# BUGS
  None known. This does not mean they do not exist though.  Please send
  bug reports and source code patches to (bugs@sqrt.ca).
