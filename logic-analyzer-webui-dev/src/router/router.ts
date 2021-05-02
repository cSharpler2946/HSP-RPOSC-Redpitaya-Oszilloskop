import { createWebHistory, createRouter } from "vue-router";

const LogicAnalyzer = import('@/views/LogicAnalyzer.vue')
const Documentation = import('@/views/Documentation.vue')

const history = createWebHistory();
const router = createRouter({
    history, 
    routes: [
        {
            path: '/',
            name: "LogicAnalyzer",
            component: LogicAnalyzer
        },
        {
            path: '/documentation',
            name: "Documentation",
            component: Documentation
        }
    ]
})

export default router;