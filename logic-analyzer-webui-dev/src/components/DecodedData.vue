<template>
    <div class="card">
        <div class="card-body">
            <select v-model="selectedAnnotationGroup" class="form-select" aria-label="Select annotation group">
                <option disabled value="">Choose annotation group</option>
                <!-- <option v-for="" :value="">
                    Some text
                </option> -->
            </select>
            <table>
                <thead>
                    <tr>
                        <th>Text</th>
                        <th>Start Index</th>
                        <th>End Index</th>
                    </tr>
                </thead>
                <tbody>
                <!--  for-each drum  -->
                </tbody>
            </table>

        </div>
    </div>
</template>

<script>
export default {
  name: 'DecodedData',
  props: {
      annotationData: { annotations: [] }
  },
  data () {
    return {
      annotationsByGroup: {},
      selectedAnnotationGroup: '',
    }
  },
  watch: {
      annotationData: {
          handler: function() {
              this.annotationsByGroup = this.groupBy(this.annotationData.annotations, "annotationClass");
              console.log("annotationsByGroup: ", this.annotationsByGroup, "Add a drop down for the keys of this object and for the selected object, show start index, end index aand annotationText in a grid.");
          },
          deep: true
      }
  },
  methods: {
      groupBy: function (xs, key) {
        return xs.reduce(function (rv, x) {
            (rv[x[key]] = rv[x[key]] || []).push(x);
            return rv;
        }, {});
    },
  }
}
</script>

<style lange="scss" scoped>

.decoded-data{
    text-align: left;
    overflow-y: auto;
    max-height: 100%;
}

</style>
