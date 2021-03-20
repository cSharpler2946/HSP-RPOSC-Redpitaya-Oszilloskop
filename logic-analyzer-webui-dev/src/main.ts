import { createApp } from 'vue'
import App from './App.vue'

// import bootstrap
import './libs/bootstrap.min.css'
import './libs/bootstrap.bundle.min.js'

// import fontawesome
import { library } from '@fortawesome/fontawesome-svg-core'
import { faPlay, faCaretDown, faCog } from '@fortawesome/free-solid-svg-icons'
import { FontAwesomeIcon } from '@fortawesome/vue-fontawesome'

// Add the fontawesome icons you want to use to the library.
// By doing so, only the used icons will get used in the productive version.
library.add(faPlay, faCaretDown, faCog)

const app = createApp(App)
app.component('font-awesome-icon', FontAwesomeIcon)
app.mount('#app')
