const express = require('express')
const router = express.Router()

const visit_controller = require('../controllers/visitController')
const map_controller = require('../controllers/mapController')

router.get('/', map_controller.send_map_data)

router.post('/', visit_controller.register_visit)


module.exports = router