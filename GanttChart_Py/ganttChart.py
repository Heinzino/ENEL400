import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import pandas as pd
from datetime import datetime

# Define category colors
detailed_category_colors = {
    "Rear System - Mechanical": "purple",
    "Rear System - Electrical": "orange",
    "Rear System - Software": "pink",
    "Front System - Electrical": "cyan",
    "Front System - Software": "blue",
    "Front System - Mechanical": "violet",
    "Testing": "red",
    "System Integration": "brown",
    "Documentation": "black"
}

# Project timeline
project_start = datetime(2024, 1, 6)
project_end = datetime(2024, 4, 7)

# Updated finalized timeline with clear milestones
finalized_timeline = {
    "Rear System - Mechanical": [
        ("Steel Frame Prototype", project_start, datetime(2024, 1, 20)),
        ("Assemble Stand", datetime(2024, 1, 21), datetime(2024, 2, 1))
    ],
    "Rear System - Electrical": [
        ("Research Components", project_start, datetime(2024, 2, 10)),
        ("First Prototype", datetime(2024, 2, 11), datetime(2024, 2, 18)),  # Feb 18: Prototype done
        ("PCB Design & Order", datetime(2024, 3, 1), datetime(2024, 3, 7)),  # Mar 7: PCB Soldered
        ("Final Electrical Testing", datetime(2024, 3, 20), datetime(2024, 3, 30))
    ],
    "Rear System - Software": [
        ("Define Software Requirements", project_start, datetime(2024, 1, 31)),
        ("Initial Implementation", datetime(2024, 2, 1), datetime(2024, 2, 20)),
        ("Software Refinement", datetime(2024, 2, 21), datetime(2024, 3, 10))
    ],
    "Front System - Electrical": [
        ("Research & Prototype", datetime(2024, 1, 15), datetime(2024, 2, 10)),
        ("PCB Design", datetime(2024, 2, 11), datetime(2024, 3, 1)),
        ("Final Testing", datetime(2024, 3, 10), datetime(2024, 3, 20))
    ],
    "Front System - Software": [
        ("Software Implementation", datetime(2024, 1, 20), datetime(2024, 2, 20)),
        ("Refinement & Debugging", datetime(2024, 2, 21), datetime(2024, 2, 28)),
        ("Final Integration Testing", datetime(2024, 3, 5), datetime(2024, 3, 15))
    ],
    "Front System - Mechanical": [
        ("Enclosure Design", datetime(2024, 3, 1), datetime(2024, 3, 10)),
        ("Fabrication", datetime(2024, 3, 11), datetime(2024, 4, 1))  # Apr 1: Final prototype & enclosures done
    ],
    "Testing": [
        ("Component Testing", datetime(2024, 3, 1), datetime(2024, 3, 10)),
        ("System Testing", datetime(2024, 3, 11), datetime(2024, 3, 25)),
        ("Final Testing & Validation", datetime(2024, 3, 26), project_end)
    ],
    "System Integration": [
        ("Integration Plan", datetime(2024, 3, 15), datetime(2024, 3, 20)),
        ("Hardware & Software Integration", datetime(2024, 3, 21), datetime(2024, 3, 30)),
        ("Final System Test", datetime(2024, 4, 1), project_end)
    ],
    "Documentation": [
        ("Design Review Presentation", datetime(2024, 2, 1), datetime(2024, 2, 6)),
        ("Requirements Specification", datetime(2024, 2, 20), datetime(2024, 2, 24)),
        ("Final Presentation & Demo", datetime(2024, 3, 30), datetime(2024, 4, 7))
    ]
}

# Convert the finalized schedule into a dataframe
final_task_schedule = []
for category, tasks in finalized_timeline.items():
    for task, start_date, end_date in tasks:
        final_task_schedule.append((category, task, start_date, end_date))

df_final = pd.DataFrame(final_task_schedule, columns=["Category", "Task", "Start Date", "End Date"])

# Plot the finalized Gantt chart
fig, ax = plt.subplots(figsize=(14, 10))

# Plot each task
for i, row in df_final.iterrows():
    ax.barh(row["Task"], (row["End Date"] - row["Start Date"]).days, left=row["Start Date"],
            color=detailed_category_colors[row["Category"]], label=row["Category"] if row["Category"] not in ax.get_legend_handles_labels()[1] else "")

# Formatting for better readability
ax.xaxis.set_major_locator(mdates.WeekdayLocator(interval=2))
ax.xaxis.set_major_formatter(mdates.DateFormatter("%b %d"))
ax.set_xlabel("Timeline", fontsize=12, loc='right')
ax.set_ylabel("Tasks", fontsize=12)
ax.set_title("ReCycle Gantt Chart", fontsize=16, fontweight="bold")
plt.xticks(rotation=45, fontsize=12)
plt.yticks(fontsize=12)

# Add milestone markers
milestones = {
    "Feb 18 - Prototype Completed": datetime(2024, 2, 18),
    "Mar 7 - PCB Soldered": datetime(2024, 3, 7),
    "Apr 1 - Final Prototype & Enclosures Done": datetime(2024, 4, 1)
}

for milestone, date in milestones.items():
    ax.axvline(date, color="black", linestyle="--", linewidth=1.5)
    ax.text(date, 0 + 1, milestone, rotation=90, verticalalignment="bottom", fontsize=12, fontweight="bold")

# Improve readability with category labels in the legend
handles, labels = ax.get_legend_handles_labels()
legend_labels = {
    "Rear System - Mechanical": "Walid",
    "Rear System - Electrical": "Namer",
    "Rear System - Software": "Marcus",
    "Front System - Electrical": "Namer",
    "Front System - Software": "Heinz",
    "Front System - Mechanical": "Walid",
    "Testing": "Nooran",
    "System Integration": "Marcus",
    "Documentation": "Mohammad"
}
ax.legend(handles, [f"{label} ({legend_labels[label]})" for label in labels], title="Categories & Assignments",
          bbox_to_anchor=(1.05, 1), loc="upper left", fontsize="small", frameon=False)

plt.tight_layout()
plt.show()
