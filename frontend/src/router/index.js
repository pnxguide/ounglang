import {
  createRouter,
  createWebHashHistory,
  createWebHistory,
} from 'vue-router';

const routes = [
  {
    path: '/',
    name: 'Home',
    component: () => import('../views/Home.vue'),
  },
  {
    path: '/editor',
    name: 'Editor',
    component: () => import('../views/Editor.vue'),
  },
];

const router = createRouter({
  history: createWebHistory(),
  routes,
});

export default router;
