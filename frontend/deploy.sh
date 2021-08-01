#!/usr/bin/env sh

# abort on errors
set -e

yarn build

cd dist

git init
git remote add origin https://github.com/cardeer/oung_editor.git
git branch -M main

git add .
git commit -m "deploy"
git push -f origin main

cd -