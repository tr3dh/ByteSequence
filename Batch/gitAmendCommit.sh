# In Git Bash : $ bash Batch/gitAmendCommit.sh

commit_and_push() {

  git commit --amend --no-edit
  git add .
  git commit --amend --no-edit
  git push --force-with-lease
}

commit_and_push "$(pwd)"