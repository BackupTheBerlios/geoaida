<MODULE class=GaImage name=fft
        tip="<p>FFT and various useful filters.</p>"
	cmd="gi_fft --filter @filter@ @wgauss@ @xbox@ @ybox@ @disc@ @lowcut@ @input@ @output@ @verbose@">
  <INPUT name=input label="Source File: " tip="Input File">
  <OUTPUT name=output depend=input label="Destination File: " tip="Output File">
  <ENUM name=filter label="Filter: " options="fft,ifft,gauss,cutbox,cutdisc,ln,exp" optionslabel="FFT2D,iFFT2D,GAUSS,CUTBOX,CUTDISC,LN,EXP" optional>
  <TEXT name=wgauss label="Width Gauss: " cmd="--wgauss @wgauss@">
  <TEXT name=xbox label="X Factor cutbox: " cmd="--xbox @xbox@">
  <TEXT name=ybox label="Y Factor cutbox: " cmd="--ybox @ybox@">
  <TEXT name=disc label="Factor cutdisc: " cmd="--disc @disc@">
  <bool name=lowcut on="--lowcut" off="">
  <bool name=verbose on="-v" off="">
</MODULE>
