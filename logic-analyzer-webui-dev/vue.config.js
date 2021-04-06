module.exports = {
  chainWebpack: config => {
    config.module.rules.delete('eslint')
  },
  publicPath: '',
  configureWebpack: {
    devtool: 'source-map'
  }
}
