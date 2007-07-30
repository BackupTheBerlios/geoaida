<MODULE class=GaImage name=ifft
        tip="<p>Inverse FFT</p>"
	cmd="gi_fft --filter ifft @polar@ @inputreal@ @inputimag@ @output@ @verbose@">
  <INPUT name=inputreal label="Source Real File: " tip="Input File">
  <INPUT name=inputimag label="Source Imag File: " tip="Input File">
  <OUTPUT name=output depend=input label="Destination File: " tip="Output File">
  <bool name=polar label="Polar" tip="Calculates polar coordinates" on="-p" off="">
  <bool name=verbose on="-v" off="">
</MODULE>
