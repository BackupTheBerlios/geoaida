<MODULE class=GaImage name=histothres
        tip="<p>Threshold an image. Threshold value is determined by splitting the image histogram at 'thres' percent of the values.</p><p>PFM - Portable Float Map</p>"
	cmd="gi_histothres @verbose@ @left@ @thres@ @input@ @output@ ">
  <INPUT name=input label="Source PFM File: " tip="PFM Input File">
  <OUTPUT name=output depend=input label="Destination PFM File: " tip="PGM Output File">
  <TEXT name=thres label="Percent:" tip="<p>Split at percent</p>" cmd="--thres @thres@">
  <bool name=left on="-l" off="">
  <bool name=verbose on="-v" off="">
</MODULE>
