<MODULE name="SplitIntoRegions" class="PFM Evaluate"
        cmd="ga_split_into_regions @input@ @labelpic@ @regions@ @minsize@ @maxsize@ @class@">
  <INPUT name=input>
  <OUTPUT name=regions depend=input cmd="@regions@" optional>
  <OUTPUT name=labelpic depend=input cmd="@labelpic@">
  <TEXT name=minsize>
  <TEXT name=maxsize>
  <TEXT name=class>
</MODULE>
