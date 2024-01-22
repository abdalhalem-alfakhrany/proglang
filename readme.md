# ProgLang: A Journey into Programming Language Development

ProgLang, a shorthand for programming language ( average C developer try to  name a variable ). While not intended for production use (at least for now), this project serves as a robust learning experience.

## Build and Experiment

The language is currently functional on my development machine running Linux (Arch), and I'm optimistic it will also function on Windows.

**Build:**
```
$ make proglang
```

**Try:**
```
$ ./build/proglang examples/main.prog
```
you can also see the ast of code by adding -ast_dump flag with type of dumping you prefer

this will give you a dot file you can render it with graphviz
```
$ ./build/proglang <filename>.prog -ast_dump graph
```

this will print to terminal directly
```
$ ./build/proglang <filename>.prog -ast_dump term
```

## Syntax Snapshot
syntax is very close to c language

##### function declaration
```c
main() {
    return 0;
}
```

#### function call
```c
inc(int x) {
    return x+1;
}

main() {
    return inc(1);
}
```

##### variable declaration
```c
int a = 10;

main() {
    int c = 20;
}
```
assignment not implemented yet (i wonder why it must bet easy 😔)

#### if else statement
```c
main() {
    if (0) {
        return 10;
    } else {
        return 9;
    }
}
```

It's important to note that numerous errors may surface during usage – expect the unexpected!

I extend my gratitude to anyone willing to contribute. Experienced individuals, your insights and expertise would be immensely valuable in shaping the evolution of ProgLang. Let's collaborate and make this journey into programming language development even more enriching!