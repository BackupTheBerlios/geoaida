<MODULE class=GaImage name=convolution
        tip="<p>Convolution</p>"
	cmd="gi_convolution @input@ @kernel@ @output@ @verbose@">
  <INPUT name=input label="Source Image: " tip="Input File">
  <INPUT name=kernel label="Kernel Image: " tip="Input File">
  <OUTPUT name=output depend=input label="Destination File: " tip="Output File">
  <bool name=verbose on="-v" off="">
</MODULE>
