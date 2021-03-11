# ValidArgumentChecker
Checks zeroth-order symbolic logical arguments for validity via the command line.

## Instructions
Type a logical argument or expression at the prompt and press `Enter`. The argument will be evaluated for logical validity (i.e. tautology via a truth table) and the result will be printed to the console.

## Syntax
` a-Z` - propositions

` ( )` - parentheses

`  ~ ` - not

`  & ` - and

`  | ` - or

`  > ` - if-then

`  = ` - if and only if

## Notes
 - Operator precedence is as shown above.
 - Spaces can be used freely to improve readability.

## Example

**Premise 1:**  Either it is cloudy outside, or it is not raining.

**Premise 2:**  It is raining.

**Conclusion:** Therefore, it is cloudy outside.

Using `&` to connect our premises and `>` to imply our conclusion, this argument can be written:

`((C | ~R) & R) > C`

## Repl
A repl exists for this project for the sake of convenience. If you'd like to check it out, follow [this link](https://repl.it/talk/share/Valid-Argument-Checker/127450).
