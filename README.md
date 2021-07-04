# molecule

### UPDATE (04. October, 2017.): There will be no updates on this engine. ###

molecule is a chess engine written in C++14 and is derivate of
[VICE](http://bluefever.net/Downloads/ViceReadMe.html).

Molecule is a bitboard chess engine that uses magic bitboards for move
generation and consists of basic search algorithms (alpha beta pruning,
quiescence search, iterative deepening and null move pruning) and transposition
table. Molecule uses very (very very) basic evaluation function (piece square
tables and material count) and because of that his playing strength is not at
the best.

The goal of this project was to learn as much as possible (in short period of
time, approx. 2.5 months) about methods and algorithms that make a chess engine.

### Try Molecule

If you wanna play against Molecule or test your own chess engine against Molecule follow these steps:

1. in folder /dependency/magic-bits run make
2. in folder /src run make
3. run ./molecule

### Info

Molecule was tested on Xubuntu 16.04. 64-bit with Arena Linux 1.1.

ELO Rating: N/A

!!! Molecule currently doesn't support opening book !!!


### Future work

I'm quite aware that Molecule's source code is far from good, but I'm happy
that it works. Because of that I'm planning on making a new version of Molecule,
written from scratch. But before that, this version need improvement in
evaluation function and support for opening book.

### Resources

On my journey I found few good resources on developing a chess engine. Most
valuable were the following:

1. [Chess Engine in C](https://www.youtube.com/playlist?list=PLZ1QII7yudbc-Ky058TEaOstZHVbT-2hg)
2. [Chess Programming Wiki](http://chessprogramming.wikispaces.com/)
3. [Bruce Moreland's Site])http://web.archive.org/web/20080216031116/http://www.seanet.com/~brucemo/topics/topics.htm)
4. [Robert Hyatt's paper on bitboards](http://www.craftychess.com/hyatt/boardrep.html)

### Credits

Molecule would not be possible without VICE. Every time I would get stuck on
development, VICE, with it's youtube playist, would clarify what is the next
step in development and how to do it. I highly recommend anyone interested in
programming a chess engine to follow development of [VICE](https://www.youtube.com/playlist?list=PLZ1QII7yudbc-Ky058TEaOstZHVbT-2hg).

I would also like to thank to Goutham Bhat for the [magic bitboards library](https://github.com/goutham/magic-bits).
