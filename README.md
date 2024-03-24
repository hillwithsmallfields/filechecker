filechecker
===========

A program to check rapidly that files that should contain text do
contain something that has the right characters for being text.  Meant
as a quick check that the files haven't been encryted.

Usage
-----

filechecker [-o file.csv | -a file.csv] file_or_dir ...

With option `-o`, the output file is truncated, and a header is written to it.

With option '-a', the output file is opened for appending, and no header is written.
