# Webguin advanced example

This example goes over a using controllers to have more power over requests. The example creates the following endpoints


- TestServer
- - `http://127.0.0.1:<PORT>/hello` `method`
- - `http://127.0.0.1:<PORT>/foo/*` `controller`

- TestController
- - `http://127.0.0.1:<PORT>/foo/hello` `method`
- - `http://127.0.0.1:<PORT>/foo/bar/*` `controller`

- TestControllerSub
- - `http://127.0.0.1:<PORT>/foo/bar/hello` `method`
- - `http://127.0.0.1:<PORT>/foo/bar/*/*` `method`
- - `http://127.0.0.1:<PORT>/foo/bar/*` `controller->execute`

Params are defined using `{}` for required ones and `[]` for optionals.
You can use types by defining it with the `:` splitter, such as `foobar:string`. [List of types](#Types), by default `string` is used.

## Order of execution

- Methods
- Controllers
- Controller execute

Once it can handle a request, it will return and skip the latter so that a request doesn't get handled twice.

## Customizing

When using `addMethod` and `addController` the templated header by default uses `Method` and `Controller` type. When you inherit from these you can use `addMethod<T>` and `addController<T>` to be able to override and have more control of the request. 
