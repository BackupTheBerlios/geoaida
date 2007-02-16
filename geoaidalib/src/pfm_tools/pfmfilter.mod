<MODULE name="FilterPFM" class="PFM Evaluate"
        cmd="pfmfilter @input@ @filter@ @output@">
  <INPUT name=input>
  <INPUT name=filter>
  <OUTPUT name=output depend=input>
</MODULE>
