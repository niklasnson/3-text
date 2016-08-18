##[3^text]

A small and tiny application that can process textfiles for use with "less great" webinterfaces for blogging.   

### Usage:
```bash
$ bin/text -t -d testfile.txt
```

Will read the testfile.txt
```bash
this is the first line
this is the second line
TEST LINE IN UPPERCASE
TEST blaha TEST blaha

detta är en annan test för att se om vi kan få en tom rad. det skulle göra livet
lite enklare...
#tag detta är en #annan #liten tag
```
Will process into:

```bash
=={  tags  }============================

tag, annan, liten { >1 min }

=={ content }===========================

this is the first line this is the second line test line in uppercase test blaha test blaha

detta är en annan test för att se om vi kan få en tom rad. det skulle göra livet lite enklare... tag detta är en annan liten tag
```
### Options
+ `-t` will collect tags from the text (must be written as #tag)
+ `-d` will downcase all text.
+ `-u` will upcase all text.


### Compile
`g++ -std=c++11 src/main.cpp -o dist/text -w`
or run makefile
