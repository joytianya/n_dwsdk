find . -type f -name "*.zip" -print0 | while IFS= read -r -d '' zipfile; do
  echo "Unzipping: $zipfile"
  target_dir="${zipfile%.zip}"
  if unzip -o "$zipfile" -d "$target_dir"; then
    echo "Unzip success. Deleting: $zipfile"
    rm -f "$zipfile"
  else
    echo "Unzip failed: $zipfile"
  fi
done
