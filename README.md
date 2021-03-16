# ValidArgumentChecker
Checks zeroth-order symbolic logical arguments for validity via the command line.

This project was written for personal educational purposes at the direction of my professor Michelle Cheatham, mostly to dip my toes into some data structures applications. As a result, the approach taken here is probably overkill, and that's okay; I certainly learned more doing it the way that I did. Feel free to spin up the Repl at the bottom of this page to run the code without having to download and compile it.

## Instructions
Type a logical argument or expression at the prompt and press `Enter`. The argument will be evaluated for logical validity (i.e. tautology via a truth table) and the result will be printed to the console.

## Syntax
`a-Z` - propositions

`0` - contradiction

`1` - tautology

`( )` - parentheses

`~` - not

`&` - and

`|` - or

`>` - if-then

`=` - if and only if

## Notes
 - Operator precedence is as shown above
 - Operators of equal precedence are read from left to right.
   - e.g. `(P > Q > R) = ((P > Q) > R)` 
 - Spaces can be used freely to improve readability.

## Example

**Premise 1:**  Either it is cloudy outside, or it is not raining.

**Premise 2:**  It is raining.

**Conclusion:** Therefore, it is cloudy outside.

Using `&` to connect our premises and `>` to imply our conclusion, this argument can be written:

`((C | ~R) & R) > C`

We can also use the `=` operator to validate the logical equivalence of two expressions:

`(~P | Q) = (P > Q)`

`(P & ~P) = 0`

## Repl
A repl exists for this project for the sake of convenience. If you'd like to check it out, follow [this link](https://repl.it/talk/share/Valid-Argument-Checker/127450).
