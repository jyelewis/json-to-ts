const {jsonToTs} = require("./jsonToTs");

describe("json-to-js", () => {
    it("Single string", async () => {
        await expect(jsonToTs("Hello")).resolves.toEqual(`export type JsonData = string;`.trim());
    });

    it("Single with backslashes", async () => {
        await expect(jsonToTs("Hello \"mate\"")).resolves.toEqual(`export type JsonData = string;`.trim());
    });

    it("Single number", async () => {
        await expect(jsonToTs(123)).resolves.toEqual(`export type JsonData = number;`.trim());
    });


    it("Single decimal number", async () => {
        await expect(jsonToTs(123.45)).resolves.toEqual(`export type JsonData = number;`.trim());
    });

    it("Single decimal as object property", async () => {
        await expect(jsonToTs({ myNum: 123.45 })).resolves.toEqual(`export type JsonData = {
  myNum: number;
};`.trim());
    });

    it("Single boolean (true)", async () => {
        await expect(jsonToTs(true)).resolves.toEqual(`export type JsonData = boolean;`.trim());
    });

    it("Single boolean (false)", async () => {
        await expect(jsonToTs(false)).resolves.toEqual(`export type JsonData = boolean;`.trim());
    });

    it("Single null", async () => {
        await expect(jsonToTs(null)).resolves.toEqual(`export type JsonData = null;`.trim());
    });

    it("Simple object", async () => {
        await expect(jsonToTs({hello: "world"})).resolves.toEqual(`
export type JsonData = {
  hello: string;
};
        `.trim());
    });

    it("Empty object", async () => {
        await expect(jsonToTs({})).resolves.toEqual(`
export type JsonData = {};
        `.trim());
    });

    it("Empty object in object", async () => {
        await expect(jsonToTs({ emptyObj: {} })).resolves.toEqual(`
export type JsonData = {
  emptyObj: {};
};
        `.trim());
    });

    it("Simple array", async () => {
        await expect(jsonToTs([1, 2, 3])).resolves.toEqual(`export type JsonData = number[];`.trim());
    });

    it("Empty array", async () => {
        await expect(jsonToTs([])).resolves.toEqual(`
export type JsonData = [];
        `.trim());
    });

    it("Empty array in object", async () => {
        await expect(jsonToTs({ emptyArr: [] })).resolves.toEqual(`
export type JsonData = {
  emptyArr: [];
};
        `.trim());
    });

    it("Array of different types", async () => {
        await expect(jsonToTs([1, 2, 3, false])).resolves.toEqual(`export type JsonData = Array<boolean | number>;`.trim());
    });

    it("Array of many types", async () => {
        await expect(jsonToTs([1, 2, 3, false, true, null, undefined, "Hi", 4])).resolves.toEqual(`export type JsonData = Array<null | boolean | number | string>;`.trim());
    });

    it("Large object", async () => {
        const obj = {
            str1: "Hello",
            num1: 123,
            bool1: true,
            str2: "world",
            num2: 456,
            bool2: false
        };

        await expect(jsonToTs(obj)).resolves.toEqual(`
export type JsonData = {
  str1: string;
  num1: number;
  bool1: boolean;
  str2: string;
  num2: number;
  bool2: boolean;
};
        `.trim());
    });

    it("Nested objects", async () => {
        const obj = {
            str1: "Hello",
            nested1: {
                nested1Str: "World",
                nested2: {
                    nested2Null: null
                }
            }
        };

        await expect(jsonToTs(obj)).resolves.toEqual(`
export type JsonData = {
  str1: string;
  nested1: {
    nested1Str: string;
    nested2: {
      nested2Null: null;
    };
  };
};
        `.trim());
    });

    it("Array of same objects", async () => {
        await expect(jsonToTs([{ "age": 1 }, { "age": 2 }])).resolves.toEqual(`
export type JsonData = Array<{
  age: number;
}>;
        `.trim());
    });

    it("Array of different objects", async () => {
        await expect(jsonToTs([{ "age": 1 }, { "name": "John" }])).resolves.toEqual(`
export type JsonData = Array<{
  age?: number;
  name?: string;
}>;
        `.trim());
    });

    it("Nested objects within array", async () => {
        const obj = [
            {
                shared1Str: "a",
                shared2Mix: 1,
                prop1: "a"
            },
            {
                shared1Str: "a",
                shared2Mix: "b",
                prop2: "a"
            },
            {
                shared1Str: "a",
                shared2Mix: false,
                prop3: "a"
            },
        ];

        await expect(jsonToTs(obj)).resolves.toEqual(`
export type JsonData = Array<{
  shared1Str: string;
  shared2Mix: boolean | number | string;
  prop1?: string;
  prop2?: string;
  prop3?: string;
}>;
        `.trim());
    });

    it("2D Array", async () => {
        const obj = [
            [1, 2, 3],
            [4, 5, 6]
        ];

        await expect(jsonToTs(obj)).resolves.toEqual(`
export type JsonData = number[][];
        `.trim());
    });

    it("3D Array", async () => {
        const obj = [
            [[1, 2], [3, 4], [5, 6]],
            [[7, 8], [9, 10], [11, 12]]
        ];

        await expect(jsonToTs(obj)).resolves.toEqual(`
export type JsonData = number[][][];
        `.trim());
    });

    it("3D Array with mixed types", async () => {
        const obj = [
            [[1, false], [3, "hi"], []],
            [[7, 8], [null, true], [11, 12]]
        ];

        await expect(jsonToTs(obj)).resolves.toEqual(`
export type JsonData = Array<null | boolean | number | string>[][];
        `.trim());
    });

    it("3D Array with mixed types and mixed depths", async () => {
        const obj = [
            [[1, false], [3, "hi"], [], 1],
            5,
            [[7, 8], [null, true], [11, 12]]
        ];

        await expect(jsonToTs(obj)).resolves.toEqual(`
export type JsonData = Array<number | Array<number | Array<null | boolean | number | string>>>;
        `.trim());
    });

    it("Example API data 1", async () => {
        const obj = {
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
        };

        await expect(jsonToTs(obj)).resolves.toEqual(`
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
        `.trim());
    });

    it("Example API data 2", async () => {
        const obj = {
            pagination: {
                results: 152,
                limit: 2,
                skipped: 0
            },
            users: [
                {
                    firstName: "John",
                    lastName: "Smith",
                    profileImage: "https://picsum.photos/200",
                    recentLogins: [1631391272, 1631393555],
                    premiumUser: false,
                    favouriteProperties: [
                        {
                            address: "123 fake street",
                            suburb: "Springfield",
                            country: "USA",
                            rating: 5
                        }
                    ]
                },
                {
                    firstName: "Jane",
                    lastName: "Doe",
                    profileImage: null,
                    premiumUser: true,
                    favouriteProperties: [
                        {
                            address: "456 second avenue",
                            suburb: "Columbus",
                            state: "OH",
                            country: "USA",
                            rating: 4
                        }
                    ]
                }
            ]
        };

        await expect(jsonToTs(obj)).resolves.toEqual(`
export type JsonData = {
  pagination: {
    results: number;
    limit: number;
    skipped: number;
  };
  users: Array<{
    firstName: string;
    lastName: string;
    profileImage: null | string;
    recentLogins?: number[];
    premiumUser: boolean;
    favouriteProperties: Array<{
      address: string;
      suburb: string;
      country: string;
      rating: number;
      state?: string;
    }>;
  }>;
};
        `.trim());
    });

});
