JSON TO TypeScript
====

Just a little hack project to keep my C skills sharp.
I don't intend to maintain this, and while it has tests, no guarantee of it's stability or security 🙃

Purpose
---
Converts JSON
```json
{
  "pagination": {
    "results": 152,
    "limit": 2,
    "skipped": 0
  },
  "users": [
    {
      "firstName": "John",
      "lastName": "Smith",
      "profileImage": "https://picsum.photos/200",
      "recentLogins": [
        1631391272,
        1631393555
      ],
      "premiumUser": false,
      "favouriteProperties": [
        {
          "address": "123 fake street",
          "suburb": "Springfield",
          "country": "USA",
          "rating": 5
        }
      ]
    },
    {
      "firstName": "Jane",
      "lastName": "Doe",
      "profileImage": null,
      "premiumUser": true,
      "favouriteProperties": [
        {
          "address": "456 second avenue",
          "suburb": "Columbus",
          "state": "OH",
          "country": "USA",
          "rating": 4
        }
      ]
    }
  ]
}
```
To a matching TypeScript type:
```typescript
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
```

Performance
-----
Designed to be highly performant with a small memory footprint.

Can convert ~135,000 files per second.

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
