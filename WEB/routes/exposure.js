const express = require('express')
const router = express.Router()
const exposure_controller = require('../controllers/exposureController')



router.get('/', exposure_controller.exposure)


module.exports = router