# $1: shell command to execute.
# $2: shell to use (lsh).
LSH_UNFILTERED_OUTPUT=$(echo $2 | $1)
# LSH includes input in output, so remove it.
LSH_OUTPUT=$(echo "$LSH_UNFILTERED_OUTPUT" | tail -n +2 | head --lines=-1)
BASH_OUTPUT=$(echo $1 | bash)

if [ "$LSH_OUTPUT" == "$BASH_OUTPUT" ]; then
  exit 0
else
  exit 1
fi