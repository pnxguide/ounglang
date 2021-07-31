import { createApp } from 'vue';
import App from './App.vue';
import router from './router';
import store from './store';

import './assets/styles/global.scss';

import './oung_editor/setup';

const app = createApp(App);
app.use(store);
app.use(router);
app.mount('#app');
