<MODULE class=GaImage name=despeckle
        tip="<p>Despeckle an image. Use the 8-pixel neighbourhood.</p><p>PFM - Portable Float Map</p>"
	cmd="gi_despeckle @size@ @labelsrc@ @labeldst@ @input@ @output@">
  <INPUT name=input label="Source PFM File: " tip="PFM Input File">
  <OUTPUT name=output depend=input label="Destination PFM File: " tip="PGM Output File">
  <TEXT name=size label="Speckle size" tip="<p>Size of region to be removed</p>" cmd="--size @size@">
  <TEXT name=labelsrc label="Label SRC" tip="<p>Value of label to be changed</p>" cmd="--labelsrc @labelsrc@">
  <TEXT name=labeldst label="Label DST" tip="<p>New label value</p>" cmd="--labeldst @labeldst@">
  <bool name=verbose on="-v" off="">
</MODULE>
