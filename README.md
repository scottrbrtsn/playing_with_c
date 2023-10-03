## Build 
`clang -lpthread prod_cons.c`

## Run
1. `./a.out` can show you the consumer starts before the producer finishes and continues until the end.
1. `./a.out | grep cC` can show you when the consumer starts to decrement.
1. `./a.out | grep fin` can show you the highest counter reached by the producer, the consumer finishes consuming at this point. 

## Test
1. `for i in {1..1000}; do ./a.out; done | sort | uniq -c`
