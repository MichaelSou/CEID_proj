const express = require('express')
const router = express.Router()
const admin_controller = require('../controllers/adminController')
const multer = require("multer");
const upload = multer({
    dest: "uploads/" // "uploads"
});

router.get('/', (req,res) => {
    res.render("admin/load_pois", {title: "Upload POI"})
})

router.post('/', upload.single("filename"), admin_controller.upload_data)

router.get('/del', admin_controller.delete_data)


router.get('/stats', admin_controller.search_stat)

router.post('/stats', admin_controller.search_stat)


module.exports = router