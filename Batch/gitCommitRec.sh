MESSAGE=${1:-"chore: update all submodules"}

commit_and_push() {

  local dir=$1
  cd "$dir"

  # Für jedes direkte Submodule rekursiv dessen submodule abwickeln
  git submodule foreach --quiet 'echo $toplevel/$sm_path' | while read subdir; do
    commit_and_push "$subdir"
  done

  #
  echo "=== $(pwd) ==="
  git add .
  if ! git diff --cached --quiet; then
    git commit -m "$MESSAGE"
    git push
  else
    echo "nichts zu committen, überspringe..."
  fi
}

commit_and_push "$(pwd)"