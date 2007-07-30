<MODULE class=GaImage name=fft
        tip="<p>FFT</p>"
	cmd="gi_fft --filter fft @polar@ @input@ @outputreal@ @outputimag@ @verbose@">
  <INPUT name=input label="Source File: " tip="Input File">
  <OUTPUT name=outputreal depend=input label="Destination Real File: " tip="Output File">
  <OUTPUT name=outputimag depend=input label="Destination Imag File: " tip="Output File">
  <bool name=polar label="Polar" tip="Calculates polar coordinates" on="-p" off="">
  <bool name=verbose on="-v" off="">
</MODULE>
