# Webguin params example

This example goes over a using params creating 2 endpoints

- `http://127.0.0.1:<PORT>/foo/*/{a}/{b}/[c]/[d]`
- `http://127.0.0.1:<PORT>/math/{a:number}/{operator:string}/{b:number}/[plusValue:number=30]`

Params are defined using `{}` for required ones and `[]` for optionals.
You can use types by defining it with the `:` splitter, such as `foobar:string`. [List of types](#Types), by default `string` is used.

## Optionals

Optional params can have a default value assigned using `=` as splitted, such as `foo=bar` or `foo:string=bar`.

## Types

- `string`
- `number`
- `bool`
- `float`
- `double`
- `long`
- `int`
