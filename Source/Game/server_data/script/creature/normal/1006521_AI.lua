--1006521	��ĥӪ��ʦ������֮�ȣ�����ս�����ٻ�
--<����ٻ���������С��,������[��ǰ]��[��ǰ]>
--  {1006551 С��A}
--  {1006552 С��B}
--  {1006553 С��C}
--  {1006554 С��D}
function c1006521_On_Enter_Combat(MapID, InstanceID, TargetID)
	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)

    --���ѡȡһ�������
	local i = math.random(1,4)

	if i == 1 then
		map.MapCreateCreature(MapID, InstanceID, 1006551, x-5, y, z+5)
	elseif i == 2 then
		map.MapCreateCreature(MapID, InstanceID, 1006552, x-5, y, z+5)
	elseif i == 3 then
		map.MapCreateCreature(MapID, InstanceID, 1006553, x-5, y, z+5)
	elseif i == 4 then
		map.MapCreateCreature(MapID, InstanceID, 1006554, x-5, y, z+5)
	end

	--ˢ�ڶ�����һ����С��
	local k = math.random(1,4)
	while k == i do
	    k = math.random(1,4)
	end

	if k == 1 then
		map.MapCreateCreature(MapID, InstanceID, 1006551, x+5, y, z+5)
	elseif k == 2 then
		map.MapCreateCreature(MapID, InstanceID, 1006552, x+5, y, z+5)
	elseif k == 3 then
		map.MapCreateCreature(MapID, InstanceID, 1006553, x+5, y, z+5)
	elseif k == 4 then
		map.MapCreateCreature(MapID, InstanceID, 1006554, x+5, y, z+5)
	end

end
aux.RegisterCreatureEvent(1006521,2,"c1006521_On_Enter_Combat")
