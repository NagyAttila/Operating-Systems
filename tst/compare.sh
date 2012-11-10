# $1: shell command to execute.
# $2: shell to use (lsh).
LSH_UNFILTERED_OUTPUT=$(echo $1 | $2 )
# LSH includes input in output, so remove it.
LSH_OUTPUT=$(echo "$LSH_UNFILTERED_OUTPUT" | sed '1d' | sed '$d')
BASH_OUTPUT=$(echo $1 | bash )

if [[ "$LSH_OUTPUT" == "$BASH_OUTPUT" ]]; then
  exit 0
else
  echo "LSH OUTPUT:"
  echo "$LSH_OUTPUT"
  echo "BASH OUTPUT:"
  echo "$BASH_OUTPUT"
  exit 1
fi
