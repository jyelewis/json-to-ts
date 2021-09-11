JSON TO TypeScript
====

Just a little hack project to keep my C skills sharp.
I don't intend to maintain this, and while it has tests, no guarantee of it's stability or security 🙃

Purpose
---
Converts JSON
```json
{
    "glossary": {
        "title": "example glossary",
        "GlossDiv": {
            "title": "S",
            "GlossList": {
                "GlossEntry": {
                    "ID": "SGML",
                    "SortAs": "SGML",
                    "GlossTerm": "Standard Generalized Markup Language",
                    "Acronym": "SGML",
                    "Abbrev": "ISO 8879:1986",
                    "GlossDef": {
                        "para": "A meta-markup language, used to create markup languages such as DocBook.",
                        "GlossSeeAlso": ["GML", "XML"]
                    },
                    "GlossSee": "markup"
                }
            }
        }
    }
}
```
To a matching TypeScript type:
```typescript
export type JsonData = {
  glossary: {
    title: string;
    GlossDiv: {
      title: string;
      GlossList: {
        GlossEntry: {
          ID: string;
          SortAs: string;
          GlossTerm: string;
          Acronym: string;
          Abbrev: string;
          GlossDef: {
            para: string;
            GlossSeeAlso: string[];
          };
          GlossSee: string;
        };
      };
    };
  };
};
```

Building
---
[Use CMake to build](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)

Usage
----
Once compiled, the binary will take JSON via stdin, and provide typescript vis stdout.
e.g.
```shell
curl https://api.my-example-app.com/books.json | json-to-ts > Books.ts
```

Tests
---
Tests are implemented in JavaScript.

*The application must be built before tests can run*

To run, cd to 'tests' then run 'yarn install && yarn test' to run all tests
