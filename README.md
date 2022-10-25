# pipex
Recreating the behavior of pipes.

Can handle multiple pipes, arguments should be ordered like:
./pipex infile "ls -l" "wc -l" outfile
Should behave like: < infile ls -l | wc -l > outfile

Can also handle here_doc:
./pipex infile "grep a1" "wc -w" outfile
Should behave like: < infile grep a1 | wc -w > outfile

- use make then proceed to test the functionality