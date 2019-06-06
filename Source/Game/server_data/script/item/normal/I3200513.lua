

--[[�ڵ�ͼħ���3000523471��������3000521935��������֮�ȣ�3000523727��ʹ����Ʒ����֮����3200513�������԰��ղ�ͬ�ĵ�ͼˢ�������NPC��
m46�����ܵ�3084408015��500, 5664, 1763ˢ��Ǭ��֮��1531011��
m44�����3084408527��812, 940, 380ˢ��ѩ��֮��1531012��
ħ���3000523471����246��15541��713ˢ�����֮�飨1531004����
����3000521935����697��11590��1804ˢ��Ѫ�֮�飨1531001����
����֮�ȣ�3000523727����1510��10460��1408ˢ�����֮�飨1531003����
ʹ����Ʒǰ�ж��Ƿ�������ˢ�µ�һ���ľ������ڣ���Χ�����Դ󣩣���ʹ����Ʒ��ʱ�����жϸ�λ�Ƿ�Ϊ1����������޷��ٻ���"
]]

pos = {}
pos[1] = {x= 246,z= 713} --m18ħ����
pos[2] = {x= 697,z= 1804} --m16�����
pos[3] = {x= 1510,z= 1408} --m19����֮��
pos[4] = {x= 500,z= 1763} --m46�����ܵ�
pos[5] = {x= 812,z= 380} --m44�����
pos[6] = {x= 246,z= 713} --m18
pos[7] = {x= 246,z= 713} --m18
pos[8] = {x= 246,z= 713} --m18
pos[9] = {x= 246,z= 713} --m18
function I3200513_QuestUsable(MapID, InstanceID, TypeID, TargetID)
	if MapID == 3000523471 then --m18
	map.MapCreateColCreature(MapID, InstanceID, 1531004, 246, 15541, 713, 1, "")
	--map.MapCreateCreature(MapID, InstanceID, 3011006, 237, 15541, 701)
	map.SetMapScriptData(MapID,InstanceID,1,3,1) --��Ӧ����������ֵ��������lua�ڲ�ѯ

	elseif MapID == 3000521935 then  --m16
	map.MapCreateColCreature(MapID, InstanceID, 1531001, 697, 11590, 1804, 1, "")
	--map.MapCreateCreature(MapID, InstanceID, 3011006, 685, 11590, 1789)
	map.SetMapScriptData(MapID,InstanceID,1,0,1)

	elseif MapID == 3000523727 then --m19
	map.MapCreateColCreature(MapID, InstanceID, 1531003, 1510, 10460, 1408, 1, "")
	--map.MapCreateCreature(MapID, InstanceID, 3011006, 1522, 10460, 1395)
	map.SetMapScriptData(MapID,InstanceID,1,0,1)

	elseif MapID == 3084408015 then --m46
	map.MapCreateColCreature(MapID, InstanceID, 1531011, 500, 5664, 1763, 1, "")
	map.SetMapScriptData(MapID,InstanceID,1,4,1)

        elseif MapID == 3084408527 then --m44 
	map.MapCreateColCreature(MapID, InstanceID, 1531012, 812, 940, 380, 1, "")
	map.SetMapScriptData(MapID,InstanceID,1,23,1)

	end
end


function I3200513_CanUse(MapID, InstanceID, TypeID, TargetID)
	local num = 0
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	if MapID == 3000523471 then --m18
		num = map.GetMapScriptData(MapID,InstanceID,1,3)
		if num == 1 then
			return 32,false
		else
			if (x > pos[1].x-50 and x < pos[1].x+50) and (z > pos[1].z-50 and z < pos[1].z+50) then
			return 0,false
			else
			return 43,false
			end
		end
	elseif MapID == 3000521935 then  --m16
		num = map.GetMapScriptData(MapID,InstanceID,1,0)
		if num == 1 then
			return 32,false
		else
			if (x > pos[2].x-50 and x < pos[2].x+50) and (z > pos[2].z-50 and z < pos[2].z+50) then
			return 0,false
			else
			return 43,false
			end
		end
	elseif MapID == 3000523727 then --m19
		num = map.GetMapScriptData(MapID,InstanceID,1,0)
		if num == 1 then
			return 32,false
		else
			if (x > pos[3].x-50 and x < pos[3].x+50) and (z > pos[3].z-50 and z < pos[3].z+50) then
			return 0,false
			else
			return 43,false
			end
		end
	elseif MapID == 3084408015 then --m46
		num = map.GetMapScriptData(MapID,InstanceID,1,4)
		if num == 1 then
			return 32,false
		else
			if (x > pos[4].x-50 and x < pos[4].x+50) and (z > pos[4].z-50 and z < pos[4].z+50) then
			return 0,false
			else
			return 43,false
			end
		end
	elseif MapID == 3084408527 then --m44
		num = map.GetMapScriptData(MapID,InstanceID,1,23)
		if num == 1 then
			return 32,false
		else
			if (x > pos[5].x-50 and x < pos[5].x+50) and (z > pos[5].z-50 and z < pos[5].z+50) then
			return 0,false
			else
			return 43,false
			end
		end
	else
		return 43,false
	end
end



aux.RegisterItemEvent(3200513, 1, "I3200513_QuestUsable")
aux.RegisterItemEvent(3200513, 0, "I3200513_CanUse")
