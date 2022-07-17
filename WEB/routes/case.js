const express = require('express')
const router = express.Router()
const case_controller = require('../controllers/caseController')



router.get('/', (req,res) => {
    res.render("case/case", {title: "Register Case"})
})

router.post('/', case_controller.register_case)

module.exports = router