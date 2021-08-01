import { createApp } from 'vue';
import App from './App.vue';
import router from './router';
import store from './store';
import axios from 'axios';
import VueAxios from 'vue-axios';

import '@mdi/font/css/materialdesignicons.min.css';
import './assets/styles/global.scss';

import './oung_editor/setup';

const app = createApp(App);
app.use(store);
app.use(router);
app.use(VueAxios, axios);
app.provide('axios', app.config.globalProperties.axios);
app.mount('#app');
