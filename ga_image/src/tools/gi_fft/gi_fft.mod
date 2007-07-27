<MODULE class=GaImage name=fft
        tip="<p>FFT, creates Image with real/magnitude and imaginary/phase data next to each other.</p>"
	cmd="gi_fft --filter @filter@ @polar@ @input@ @output@ @verbose@">
  <INPUT name=input label="Source File: " tip="Input File">
  <OUTPUT name=output depend=input label="Destination File: " tip="Output File">
  <ENUM name=filter label="Transform: " options="fft,ifft" optionslabel="FFT2D,iFFT2D">
  <bool name=polar label="Polar" tip="Calculates polar coordinates" on="-p" off="">
  <bool name=verbose on="-v" off="">
</MODULE>
