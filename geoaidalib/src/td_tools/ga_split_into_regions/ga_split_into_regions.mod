<MODULE name="SplitIntoRegions" class="GeoAIDA"
        cmd="ga_split_into_regions @input@ @labelpic@ @regions@ @minsize@ @maxsize@ @class@">
  <INPUT name=input>
  <OUTPUT name=regions depend=input cmd="@regions@" optional>
  <OUTPUT name=labelpic depend=input cmd="@labelpic@">
  <TEXT name=minsize optional>
  <TEXT name=maxsize optional>
  <TEXT name=class optional>
</MODULE>
