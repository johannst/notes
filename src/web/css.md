# css

## selector

`.moose` element with class
```html
<div class = "moose"></div>  // selected
<div class = "bar"></div>    // NOT selected
```

`.moose.bar` element with multiple classes
```html
<div class = "moose bar"></div>  // selected
<div class = "bar"></div>        // NOT selected
```

`.moose .bar` descendant element with classes
```html
<div class = "moose">
    <div class = "bar"></div>  // selected
</div>
<div class = "bar"></div>      // NOT selected
```

`p` specific element
```html
<p></p>      // selected
<div></div>  // NOT selected
```

`p.bar` specific element with class
```html
<p class = "bar"></p>  // selected
<p class = "foo"></p>  // NOT selected
```

`p,div` any element
```html
<p></p>      // selected
<div></div>  // selected
<a></a>      // NOT selected
```

`div p` descendant element of other element
```html
<div><p></p></div>           // selected
<div><ul><p></p></ul></div>  // NOT selected
```

`div > o` direct descendant element of other element
```html
<div><p></p></div>           // selected
<div><ul><p></p></ul></div>  // NOT selected
```
