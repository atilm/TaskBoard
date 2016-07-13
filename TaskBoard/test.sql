.mode column
.headers on

SELECT tasks.name, SUM(records.time) FROM projects
 JOIN tasks ON projects.id = tasks.project
 JOIN records ON tasks.id = records.task
 WHERE projects.name = "Project X"
 GROUP BY tasks.id";