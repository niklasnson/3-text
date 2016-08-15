# 3-text

a small and tiny application that can process textfiles. 

### usage: 
```bash
$ dist/text -t -d testfile.txt
```
which will give an output like: 
```bash
--{ tags }--

#tag, #annan, #liten

--{ body }--

this is the first line this is the second line test line in uppercase test blaha test blaha

detta är en annan test för att se om vi kan få en tom rad. det skulle göra livet lite enklare... tag detta är en annan liten tag
```
### options
+ `-t` will collect tags from the text (must be written as #tag) 
+ `-d` will downcase all text. 
+ `-u` will upcase all text. 
