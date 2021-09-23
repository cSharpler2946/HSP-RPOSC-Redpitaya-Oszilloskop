<template>
    <div class="card">
        <div class="card-body decoded-data">
            <select v-model="selectedAnnotationGroup" class="form-select" aria-label="Select annotation group">
                <option disabled value="">Choose annotation group</option>
                <option v-for="annotationGroup in Object.keys(annotationsByGroup)" :value="annotationGroup" :key="annotationGroup">
                    {{ annotationGroup }}
                </option>
            </select>
            <table class="table table-bordered table-striped">
                <thead>
                    <tr>
                        <th>Text</th>
                        <th>Start</th>
                        <th>End</th>
                    </tr>
                </thead>
                <tbody>
                <tr v-for="item in annotationsByGroup[selectedAnnotationGroup]" :key="item">
                    <td>{{item.annotationText}}</td>
                    <td>{{item.start}}</td>
                    <td>{{item.end}}</td>
                </tr>
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
    overflow-y: auto;
    max-height: 100vh;
}

</style>
