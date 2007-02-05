<MODULE class=GaImage name=localmax
        tip="<p>Calculate local maxima, thres is a hard threshold.</p><p>PFM - Portable Float Map</p>"
	cmd="gi_localmax @verbose@ @thres@ @input@ @output@ @outvalues@">
  <INPUT name=input label="Source PFM File: " tip="PFM Input File">
  <OUTPUT name=output depend=input label="Destination PFM File: " tip="PGM Output File">
  <OUTPUT name=outvalues depend=input label="outvalues file: " tip="Outvalues File">
  <TEXT name=thres label="Threshold:" tip="<p>Treshold</p>" cmd="--thres @thres@">
  <bool name=verbose on="-v" off="">
</MODULE>
