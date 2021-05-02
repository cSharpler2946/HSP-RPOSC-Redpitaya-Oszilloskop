import { RouteRecordRaw, createWebHistory, createWebHashHistory, createRouter } from "vue-router";
import LogicAnalyzer from '../views/LogicAnalyzer.vue';

const routes: Array<RouteRecordRaw> = [
    {
        path: '/',
        name: "LogicAnalyzer",
        component: LogicAnalyzer
    },
    {
        path: '/documentation',
        name: "Documentation",
        component: () => import('../views/Documentation.vue')
    }
]

const router = createRouter({
    history: createWebHashHistory(process.env.BASE_URL),
    routes
})


export default router;