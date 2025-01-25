i=-1
while (( i++ < 5000 )); do
  cp ./disk-files/foo.pdf "./disk-files/foo$i.pdf"
done
