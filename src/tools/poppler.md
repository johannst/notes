# poppler

```sh
pdfseparate [opts] <input> <output-pattern>
  -f N   Index of first page to extract.
  -l N   Index of last page to extract.

         Use %d in output pattern if extracting multiple pages.
         If -f is absent, it defaults to page 1.
         If -l is absent, it defaults to the last page.

pdfunite <input> [<input>] <output>
```


## Example: split pdfs

Extract pages from an input pdf.
```sh
# Extract single page.
pdfseparate -f 2 -l 2 2017-statement.pdf foo.pdf

# Extract multiple pages.
pdfseparate -f 2 -l 3 2017-statement.pdf foo%d.pdf
```

## Example: merge pdfs

Merge multiple pdf input files into a single output file.
```sh
pdfunite 201704-statement.pdf 201705-statement.pdf 2017-statement.pdf
```

# References
* https://poppler.freedesktop.org/
