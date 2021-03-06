package main

import (
	"io"
	"os/exec"
	"strconv"
	"log"
	"io/ioutil"
	"github.com/gofiber/fiber/v2"
	"github.com/gofiber/fiber/v2/middleware/cors"
	"time"
	"os"
)

type ProgramRequest struct {
    	Program string `json:"program"`
    	Input   string `json:"input"`
}

func main() {
  	app := fiber.New()

	app.Use(cors.New(cors.Config{
		AllowOrigins: "https://oung-editor.vercel.app, https://oung-editor.vercel.app/editor",
		AllowHeaders: "Access-Control-Allow-Origin, Origin, Content-Type, Accept",

	}))

	app.Post("/run", func (c *fiber.Ctx) error {
		var err error

		body := new(ProgramRequest)

		if err = c.BodyParser(body); err != nil {
			return err
		}

		currentTime := strconv.FormatInt(time.Now().Unix(), 10)
		filename := "/tmp/" + currentTime + ".oung"
		if err = ioutil.WriteFile(filename, []byte(body.Program), 0644); err != nil {
			return err
		}

		log.Println(filename)

		cmd := exec.Command("./oung", filename)
		stdin, err := cmd.StdinPipe()
		if err != nil {
			return err
		}

		go func() {
			defer stdin.Close()
			io.WriteString(stdin, body.Input)
		}()

		result, err := cmd.CombinedOutput()
		if err != nil {
			return err
		}

		if err = os.Remove(filename); err != nil {
			return err
		}

		return c.SendString(string(result))
	})

	port := os.Getenv("PORT")
	log.Fatal(app.Listen(":"+port))
}