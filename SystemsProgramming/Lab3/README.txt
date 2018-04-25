The relationship between time and buffer size seems almost linear based on the graph.
While this seems counterintuitive (since the same amount of data needs to be read and
written either way), it is clearly the case that something happens to influence the
access time. It is clearly the case that, to a point, the larger the buffer, the
shorter the copy time. 

It could have something to do with the physical distance that the data must travel.
If the distance between the source of the data and its destination on physical
hardware can be represented as d, and the time it takes to send a bit of information
across that hardware (distance d) is t, then it may take only time t to send the
entire buffer of information. Therefore if the number of times it takes to traverse
the distance (based on the buffer size) is n, then it will take nt amount of time
to copy the data. This is linear with n (which is directly related to buffer size)
which is what the data showed. Though this explaination requires it to be true that
the the time required to send a buffer is not related to the size of the buffer. I
can't think of any reason that such a think could be true, but it would fit the data
and just because I can't think of it, doesn't mean it's impossible. 

Alternatively, it could be the case that read and/or write operations are slow,
meaning that the more times you call them, the more time it will take proportional
to your data size. Even if this is the case, it is still necessary to read and write
each bit of data. This means that if the time changes with the number of times read
and write are executed then it is due to fluff in the read and write functions.
Essentially it could be due to the setting up required by the functions and not by
the actual reading or writing.
